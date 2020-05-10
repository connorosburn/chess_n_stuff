import React from 'react';

function EndReport(props) {

    const report = (subject) => {
        if(subject) {
            return <p className="end-text">{subject}</p>
        }
    }

    const renderReport = () => {
        if(props.endState) {
            return (
                <div>
                        {report(props.endState.condition)}
                        {report(props.pieceColor(props.endState.winner))}
                </div>
            );
        }
    }

    return (
        <div>
            {renderReport()}
        </div>
    );
}

export default EndReport;

